import sqlite3
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import uvicorn
from contextlib import asynccontextmanager

from db_manager import DBManager
from docker_manager import DockerManager

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