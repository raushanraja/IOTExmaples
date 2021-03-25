import asyncio
import serial
from concurrent.futures import ThreadPoolExecutor

exit=False
async def ainput(prompt: str = "") -> str:
    with ThreadPoolExecutor(1, "AsyncInput") as executor:
        return await asyncio.get_event_loop().run_in_executor(executor, input, prompt)

async def read_uart(uart ):
    global exit
    while not exit:
        if uart.in_waiting:
            print(uart.read_all().decode('utf-8').strip())
        await asyncio.sleep(1)

async def write_uart(uart):
    global exit
    while not exit:
        x =await ainput("")
        if(x== 'exit'):
            exit = True
        x+='\r'
        uart.write(bytes(x,'ascii'))
        await asyncio.sleep(1)



async def main():
    uart = serial.Serial("/dev/ttyAMA0", baudrate = 9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=1)
    task_read = asyncio.create_task(read_uart(uart))
    task_write = asyncio.create_task(write_uart(uart))

    await task_read
    await task_write
    print("Exiting")

asyncio.run(main())

