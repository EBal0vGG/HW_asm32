import subprocess
from os import system

def run_integral(params: str):
    # Construct the command
    command = ["./integral"] + params.split()

    # Run the command and capture the output
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    # Decode the output from bytes to string
    stdout = stdout.decode('utf-8')
    # Return the output
    return float(stdout.split()[0])


if run_integral("--test-root f4:f5:1:2:0.00000001:1.5") == 1.5:
    print("1st test passed")
else:
    print("1st test failed")

if run_integral("--test-root f6:f5:-0.5:0.5:0.00000001:0") == 0:
    print("2nd test passed")
else:
    print("2nd test failed")

if run_integral("--test-integral f6:1:2:0.00000001:3") == 3:
    print("3rd test passed")
else:
    print("3rd test failed")