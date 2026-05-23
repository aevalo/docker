# BASE_IMAGE can be specified with podman/docker build --build-arg BASE_IMAGE=image:tag.
ARG BASE_IMAGE=ubuntu:24.04
# RUST_TOOLCHAIN can be specified with podman/docker build --build-arg RUST_TOOLCHAIN=toolchain.
#ARG RUST_TOOLCHAIN="1.95.0-x86_64-unknown-linux-gnu" - unsure why this version does not work
ARG RUST_TOOLCHAIN

# Stage 1: Build Environment
FROM ${BASE_IMAGE} AS build-stage
# Install build tools (e.g., Maven, Gradle)
# Copy source code
# Build commands (e.g., compile, package)

#VOLUME /workspace
COPY --exclude=rust2cpp/build --exclude=rust2cpp/rust/target --exclude=rust2cpp/rust2cpp-bin rust2cpp /workspace/rust2cpp

# Install dependencies
RUN apt update && apt upgrade --yes --autoremove && \
    apt install --no-install-recommends --yes \
    build-essential ca-certificates cmake rustup corrosion git curl && \
    rustup install 1.95.0 --no-self-update

WORKDIR /workspace/rust2cpp
# Configure the project in Release mode
RUN cmake -B build -DRust_TOOLCHAIN=${RUST_TOOLCHAIN:-1.95.0-x86_64-unknown-linux-gnu} -DCMAKE_BUILD_TYPE=Release

# Compile the project
RUN cmake --build build

# Test the project
RUN cd rust && cargo test && cd .. && ctest --test-dir build

# Install the release build and strip debuginfo
RUN cmake --install build --prefix rust2cpp-bin --strip

# Stage 2: Runtime environment
FROM ${BASE_IMAGE} AS final-stage

RUN apt update && apt upgrade --yes --autoremove

# Copy application artifacts from the build stage (e.g., JAR file)
COPY --from=build-stage /workspace/rust2cpp/rust2cpp-bin /root/.local

# Setup PATH
ENV PATH="/root/.local/bin:$PATH"
ENV LD_LIBRARY_PATH="/root/.local/lib"

# Define runtime configuration (e.g., CMD, ENTRYPOINT)
#CMD ["rustup", " toolchain list"]
CMD ["cpp-exe-shared"]
