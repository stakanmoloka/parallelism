import argparse
import threading
import queue
import time
import cv2
from ultralytics import YOLO

frame_q = queue.Queue(maxsize=1000)
result_q = queue.Queue()
stop_flag = threading.Event()

def load_video_info(video_path):
    cap = cv2.VideoCapture(video_path)
    total = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    fps = cap.get(cv2.CAP_PROP_FPS)
    cap.release()
    return total, fps

def read_video(video_path):
    cap = cv2.VideoCapture(video_path)
    i = 0
    while cap.isOpened():
        success, frame = cap.read()
        if not success:
            print("End of video")
            break
        frame_q.put((frame, i))
        i += 1
        time.sleep(0.0001)
    stop_flag.set()
    cap.release()

def write_video(save_path, total_frames, fps):
    current_thread = threading.current_thread()
    frames_out = [None] * total_frames

    while not getattr(current_thread, "should_stop", False):
        try:
            img, idx = result_q.get(timeout=1)
            frames_out[idx] = img
        except queue.Empty:
            continue

    h, w = frames_out[0].shape[:2]
    writer = cv2.VideoWriter(save_path, cv2.VideoWriter_fourcc(*'mp4v'), fps, (w, h))
    for frm in frames_out:
        writer.write(frm)
    writer.release()


def detector_thread(model_path="yolov8n-pose.pt"):
    model = YOLO(model_path, verbose=False)
    while True:
        try:
            img, idx = frame_q.get(timeout=1)
            rendered = model.predict(source=img, device='cpu')[0].plot()
            result_q.put((rendered, idx))
        except queue.Empty:
            if stop_flag.is_set():
                break


def process_video(input_path, output_path, thread_count):
    total_frames, fps = load_video_info(input_path)

    reader = threading.Thread(target=read_video, args=(input_path,))
    writer = threading.Thread(target=write_video, args=(output_path, total_frames, fps))
    detectors = [threading.Thread(target=detector_thread) for _ in range(thread_count)]

    start = time.monotonic()

    reader.start()
    writer.start()
    for d in detectors:
        d.start()

    reader.join()
    for d in detectors:
        d.join()
    writer.should_stop = True
    writer.join()

    print(f"time {time.monotonic() - start:.2f} sec")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_video', type=str, default='input.mp4')
    parser.add_argument('--output_video', type=str, default='output.mp4')
    parser.add_argument('--num_threads', type=int, default=4)
    args = parser.parse_args()

    process_video(args.input_video, args.output_video, args.num_threads)

if __name__ == "__main__":
    main()
