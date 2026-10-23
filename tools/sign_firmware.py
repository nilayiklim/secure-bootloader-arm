import hashlib
import os
import struct
import sys
from ecdsa import SigningKey, SECP256k1

def generate_keys():
    private_key = SigningKey.generate(curve=SECP256k1)
    public_key = private_key.get_verifying_key()
    return private_key, public_key

def sign_firmware(firmware_path, output_path):
    
    private_key, public_key = generate_keys()

    try:
        with open(firmware_path, "rb") as f:
            firmware_data = f.read()
    except FileNotFoundError:
        print(f"Error: '{firmware_path}' not found. Please compile the project first using 'make'.")
        sys.exit(1)

    firmware_hash = hashlib.sha256(firmware_data).digest()
    
    signature = private_key.sign(firmware_hash)
    
    firmware_size = len(firmware_data)

    metadata = struct.pack("<I", firmware_size) + firmware_hash + signature

    signed_firmware = firmware_data + metadata

    with open(output_path, "wb") as f:
        f.write(signed_firmware)

    print("Success! Firmware signed securely.")
    print(f"Original Firmware Size: {firmware_size} bytes")
    print(f"Total Payload Size (incl. metadata): {len(signed_firmware)} bytes")


if __name__ == "__main__":
    sign_firmware("firmware.bin", "firmware_signed.bin")