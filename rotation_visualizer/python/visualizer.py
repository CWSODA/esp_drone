import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt
from scipy.spatial.transform import Rotation

def find_serial_port():
    ports = list(serial.tools.list_ports.comports())

    if not ports:
        raise RuntimeError("No serial devices found")

    for p in ports:
        print(f"Found: {p.device} ({p.description})")

    # Prefer USB serial devices
    for p in ports:
        if "USB" in p.description or "UART" in p.description:
            return p.device

    # Otherwise use first device
    return ports[0].device


port = find_serial_port()
print(f"Connecting to {port}")
ser = serial.Serial(
    port,
    baudrate=115200,
    timeout=1
)

fig = plt.figure()
ax = fig.add_subplot(projection="3d")

def draw_axes(q):
    # scipy uses [x,y,z,w]
    r = Rotation.from_quat(q)

    # Rotate local axes into world frame
    axes = r.apply([
        [1,0,0],
        [0,1,0],
        [0,0,1]
    ])

    colours = ["r", "g", "b"]

    for v, c in zip(axes, colours):
        ax.quiver(
            0, 0, 0,
            v[0], v[1], v[2],
            color=c,
            length=1
        )

    ax.set_xlim(-1,1)
    ax.set_ylim(-1,1)
    ax.set_zlim(-1,1)

    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")

def draw_vec(v, color):
    ax.quiver(
        0,0,0,
        v[0], v[1], v[2],
        color=color,
        length = 0.5,
    )

q = [1,0,0,0]
a = [0,0,0]
m = [0,0,0]
while True:
    line = ser.readline().decode(errors="ignore").strip()

    if not line:
        continue
    
    try:
        cmd, data = line.split(":")

        if cmd == 'q':
            qw, qx, qy, qz = map(float, data.split(","))
            q = [qx, qy, qz, qw] # scipy expects x,y,z,w
        elif cmd == 'a':
            x, y, z = map(float, data.split(","))
            a = [x, y, z]
        elif cmd == 'm':
            mx, my, mz = map(float, data.split(","))
            m = [mx, my, mz]

    except Exception as e:
        print("Bad packet:", line, e)

    ax.clear()
    draw_axes(q)
    draw_vec(a, "y")
    draw_vec(m, "cyan")
    plt.pause(0.00001)