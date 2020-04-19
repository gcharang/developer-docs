FROM node:alpine

EXPOSE 8080

RUN mkdir -p /app/

WORKDIR /app/

ENTRYPOINT [ "./entrypoint-vuepress-dev.sh" ]