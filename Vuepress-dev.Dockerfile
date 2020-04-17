FROM node:alpine

EXPOSE 8080

RUN mkdir -p /app/

WORKDIR /app/

ENTRYPOINT yarn install && yarn docs:dev
