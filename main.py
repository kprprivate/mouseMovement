from modules import *

if __name__ == "__main__":
    mouseMovement = mainFunction()

    for i in range(4):
        mouseMovement.move(100, 0)
        time.sleep(0.1)