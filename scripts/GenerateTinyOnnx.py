"""Generate a deterministic ONNX ReLU contract fixture using only Python's standard library.

This writes protobuf wire fields, not a trained detector. Schema: ONNX ModelProto,
GraphProto, NodeProto, ValueInfoProto. Input/output float32 [1,3,2,2], opset 13.
"""
from pathlib import Path
import hashlib
import sys


def varint(value):
    result = bytearray()
    while value > 127:
        result.append((value & 127) | 128)
        value >>= 7
    result.append(value)
    return bytes(result)


def integer(field, value):
    return varint(field << 3) + varint(value)


def message(field, value):
    if isinstance(value, str):
        value = value.encode("utf-8")
    return varint((field << 3) | 2) + varint(len(value)) + value


def value_info(name):
    shape = b"".join(message(1, integer(1, d)) for d in [1, 3, 2, 2])
    tensor = integer(1, 1) + message(2, shape)  # FLOAT element type
    return message(1, name) + message(2, message(1, tensor))


def generate():
    node = message(1, "input") + message(2, "output") + message(3, "relu") + message(4, "Relu")
    graph = message(1, node) + message(2, "cvlab_relu")
    graph += message(11, value_info("input")) + message(12, value_info("output"))
    opset = integer(2, 13)
    return integer(1, 7) + message(2, "cvlab-stdlib") + message(7, graph) + message(8, opset)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit("usage: GenerateTinyOnnx.py output.onnx")
    destination = Path(sys.argv[1])
    destination.parent.mkdir(parents=True, exist_ok=True)
    data = generate()
    destination.write_bytes(data)
    print(f"Generated {len(data)} bytes; SHA256={hashlib.sha256(data).hexdigest()}")
