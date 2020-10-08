FROM nginx:alpine

RUN rm -f /usr/share/nginx/html/*

COPY gitlab/index.html /usr/share/nginx/html/index.html
COPY emscripten/build/verovio-toolkit.js /usr/share/nginx/html/verovio-toolkit.js
