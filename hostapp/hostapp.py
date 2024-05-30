import serial
import time
import win32gui
import string
import psutil
import win32process
import pyaudio

MAX_LINE_LENGTH = 10

titles = set()
debug = False


def main():
    global titles
    current_titles = set()
    win32gui.EnumWindows(getWindows, current_titles)
    diff = current_titles - titles
    if not diff:
        time.sleep(1)
        return

    title = list(diff)[0]
    if title.count(" - ") >= 1:
        # Spotify song??
        parts = title.split(" - ")
        artist = parts[0]
        song = parts[1]
        write_read(song + "\n" + artist + "\n" + get_output_device())
    else:
        print(f"Ignoring window: {title}")

    titles = current_titles


def getWindows(hwnd, output: set):
    pid = win32process.GetWindowThreadProcessId(hwnd)
    process_name = psutil.Process(pid[-1]).name()
    if "spotify" in process_name.lower():
        title = win32gui.GetWindowText(hwnd)
        if title:
            output.add(title)
    else:
        if debug:
            print(f"Ignoring {process_name=} {pid=} {hwnd=}")

def get_output_device() -> str:
    p = pyaudio.PyAudio()
    device_info = p.get_default_output_device_info()
    return device_info['name'].replace("Headphones ", "").replace("Speakers ", "").strip("() ")

def prep_text(x: str):
    lines = x.split("\n")
    return "\n".join([l if len(l) < MAX_LINE_LENGTH else l[:MAX_LINE_LENGTH] for l in lines])


def write_read(x: str):
    x = prep_text(x)
    print("sending: " + x)
    try:
        arduino = serial.Serial(port="COM7", baudrate=9600, timeout=0.1)
        arduino.write(bytes(x, "utf-8"))
        arduino.close()
    except Exception as exc:
        print("Failed to send: " + str(exc))


if __name__ == "__main__":
    win32gui.EnumWindows(getWindows, titles)
    write_read("Hello ;)\n\n" + string.ascii_uppercase[:MAX_LINE_LENGTH])
    while True:
        main()
