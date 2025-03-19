import sqlite3
from datetime import datetime

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