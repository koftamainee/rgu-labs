FROM debian:latest AS builder

RUN apt-get update && apt-get install -y clang cmake ninja-build build-essential

WORKDIR /app

COPY . .

RUN make build_linux

FROM debian:bookworm-slim


WORKDIR /app

COPY --from=builder /app/build/x86_64/ /app/

ENTRYPOINT ["/app/rgu-labs"]
