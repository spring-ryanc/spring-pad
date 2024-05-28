import serial
import time
import win32gui
import string
import psutil
import win32process

MAX_LINE_LENGTH = 21

arduino = serial.Serial(port="COM5", baudrate=9600, timeout=0.1)
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
        write_read(song + "\n" + artist)
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


def prep_text(x: str):
    lines = x.split("\n")
    return "\n".join([l if len(l) < MAX_LINE_LENGTH else l[:MAX_LINE_LENGTH] for l in lines])


def write_read(x: str):
    x = prep_text(x)
    print("sending: " + x)
    arduino.write(bytes(x, "utf-8"))


if __name__ == "__main__":
    win32gui.EnumWindows(getWindows, titles)
    write_read("Hello ;)\n\n" + string.ascii_uppercase[:MAX_LINE_LENGTH])
    while True:
        main()
