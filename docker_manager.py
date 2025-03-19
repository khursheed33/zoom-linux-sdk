import os
import docker
from docker.errors import DockerException,ImageNotFound

class DockerManager:
    def __init__(self):
        self.client = docker.from_env()
        self.image_name = "zoom-sdk-ubuntu"

    def build_image(self):
        try:
            # Check if the image already exists
            self.client.images.get(self.image_name)
            print(f"Image '{self.image_name}' already exists, skipping build.")
        except ImageNotFound:
            print(f"Building Docker image '{self.image_name}'...")
            try:
                # Build the image from the Dockerfile in the root directory
                self.client.images.build(path=".", tag=self.image_name, rm=True)
                print(f"Image '{self.image_name}' built successfully.")
            except DockerException as e:
                raise Exception(f"Failed to build Docker image: {str(e)}")

    def start_container(self, meeting_id, meeting_password):
        try:
            container = self.client.containers.run(
                image=self.image_name,
                name=meeting_id,
                detach=True,
                environment={
                    "MEETING_ID": meeting_id,
                    "MEETING_PASSWORD": meeting_password,
                    "ZOOM_SDK_KEY": os.getenv("ZOOM_SDK_KEY"),
                    "ZOOM_SDK_SECRET": os.getenv("ZOOM_SDK_SECRET"),
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