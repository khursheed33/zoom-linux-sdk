import sqlite3
from datetime import datetime
import docker
from docker.errors import DockerException, ImageNotFound
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import uvicorn
from contextlib import asynccontextmanager

# DBManager class (unchanged)
class DBManager:
    def __init__(self, db_path="zoom_containers.db"):
        self.db_path = db_path
        self._init_db()

    def _init_db(self):
        with sqlite3.connect(self.db_path) as conn:
            conn.execute("""
                CREATE TABLE IF NOT EXISTS containers (
                    container_id TEXT PRIMARY KEY,
                    meeting_id TEXT,
                    meeting_password TEXT,
                    status TEXT,
                    created_at TEXT
                )
            """)

    def add_container(self, container_id, meeting_id, meeting_password):
        with sqlite3.connect(self.db_path) as conn:
            conn.execute("""
                INSERT INTO containers (container_id, meeting_id, meeting_password, status, created_at)
                VALUES (?, ?, ?, ?, ?)
            """, (container_id, meeting_id, meeting_password, "running", datetime.now().isoformat()))

    def get_container(self, container_id):
        with sqlite3.connect(self.db_path) as conn:
            cursor = conn.execute("SELECT * FROM containers WHERE container_id = ?", (container_id,))
            row = cursor.fetchone()
            if row:
                return {"container_id": row[0], "meeting_id": row[1], "meeting_password": row[2], "status": row[3], "created_at": row[4]}
            return None

    def update_status(self, container_id, status):
        with sqlite3.connect(self.db_path) as conn:
            conn.execute("UPDATE containers SET status = ? WHERE container_id = ?", (status, container_id))

    def delete_container(self, container_id):
        with sqlite3.connect(self.db_path) as conn:
            conn.execute("DELETE FROM containers WHERE container_id = ?", (container_id,))

# DockerManager class
class DockerManager:
    def __init__(self):
        self.client = docker.from_env()
        self.image_name = "zoom-sdk-ubuntu"

    def build_image(self):
        try:
            self.client.images.get(self.image_name)
            print(f"Image '{self.image_name}' already exists, skipping build.")
        except ImageNotFound:
            print(f"Building Docker image '{self.image_name}'...")
            try:
                self.client.images.build(path=".", tag=self.image_name, rm=True)
                print(f"Image '{self.image_name}' built successfully.")
            except DockerException as e:
                raise Exception(f"Failed to build Docker image: {str(e)}")

    def start_container(self, meeting_id, meeting_password):
        try:
            container = self.client.containers.run(
                self.image_name,
                detach=True,
                environment={
                    "MEETING_ID": meeting_id,
                    "MEETING_PASSWORD": meeting_password,
                    "ZOOM_SDK_KEY": "drlxHE5GRHSleJPIkLooOQ",
                    "ZOOM_SDK_SECRET": "h5xAhOqBy90qXUhJ4jI41oj46zrjtE54"
                }
            )
            return container.id
        except DockerException as e:
            raise Exception(f"Failed to start container: {str(e)}")

    def get_status(self, container_id):
        try:
            container = self.client.containers.get(container_id)
            return container.status
        except DockerException:
            return "not_found"

    def stop_container(self, container_id):
        try:
            container = self.client.containers.get(container_id)
            container.stop()
            container.remove()
            return True
        except DockerException:
            return False

# Lifespan handler for startup
@asynccontextmanager
async def lifespan(app: FastAPI):
    # Startup: Build the Docker image
    docker_mgr.build_image()
    yield  # Application runs here
    # Shutdown: Optional cleanup (e.g., stop running containers)
    print("Shutting down...")

# FastAPI app with lifespan
app = FastAPI(lifespan=lifespan)
docker_mgr = DockerManager()
db_mgr = DBManager()

class MeetingRequest(BaseModel):
    meeting_id: str
    meeting_password: str

@app.post("/start-meeting/")
async def start_meeting(request: MeetingRequest):
    try:
        container_id = docker_mgr.start_container(request.meeting_id, request.meeting_password)
        db_mgr.add_container(container_id, request.meeting_id, request.meeting_password)
        return {"container_id": container_id}
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Error starting meeting: {str(e)}")

@app.get("/status/{container_id}")
async def get_status(container_id: str):
    container_info = db_mgr.get_container(container_id)
    if not container_info:
        raise HTTPException(status_code=404, detail="Container not found")
    status = docker_mgr.get_status(container_id)
    db_mgr.update_status(container_id, status)
    return {"container_id": container_id, "status": status}

@app.delete("/stop-meeting/{container_id}")
async def stop_meeting(container_id: str):
    container_info = db_mgr.get_container(container_id)
    if not container_info:
        raise HTTPException(status_code=404, detail="Container not found")
    if docker_mgr.stop_container(container_id):
        db_mgr.delete_container(container_id)
        return {"message": f"Container {container_id} stopped and removed"}
    else:
        raise HTTPException(status_code=500, detail="Failed to stop container")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)