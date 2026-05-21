import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import subprocess

class BuildHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path.endswith(('.c', '.h', 'SConstruct')):
            print(f"File {event.src_path} changed. Rebuilding...")
            subprocess.run(["scons"])

observer = Observer()
observer.schedule(BuildHandler(), path='.', recursive=True)
observer.start()
try:
    while True: time.sleep(1)
except KeyboardInterrupt:
    observer.stop()
observer.join()
