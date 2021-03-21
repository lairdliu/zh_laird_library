#! /bin/bash

echo '1. generate grpc sources for outer/grpc.proto'
protoc -I ./proto/ --grpc_out=../ --plugin=protoc-gen-grpc=./executor_cpp/grpc_cpp_plugin ./proto/outer/grpc.proto

echo '2. generate proto sources for common'
protoc -I ./proto/ --cpp_out=../ ./proto/common/*.proto

echo '3. generate proto sources for outer'
protoc -I ./proto/ --cpp_out=../ ./proto/outer/*.proto
