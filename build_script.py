import argparse
import subprocess
import os

def run_command(command):
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        raise RuntimeError(f"Command '{command}' failed with exit code {result.returncode}")

def main(training):
    try:
        # Run the first Python script if training is True 
        if training:
            print("Running VAE_model_training.py...")
            run_command("python3 Bayesian/VAE/VAE_model_training.py")
        else:
            print("Skipping VAE_model_training.py...")

        # Run the second Python script
        print("Running VAE_model_generator.py...")
        run_command("python3 Bayesian/VAE/VAE_model_generator.py")

        # Run CMake and Make
        print("Running cmake and make...")
        os.makedirs('build', exist_ok=True)
        os.chdir('build')
        run_command("cmake ..")
        run_command("make")

        print("Build successful!")

    except RuntimeError as e:
        print(e)
        print("Build failed.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Build script')
    parser.add_argument('-training', action='store_true', help='Train the VAE model')
    args = parser.parse_args()
    main(args.training)
