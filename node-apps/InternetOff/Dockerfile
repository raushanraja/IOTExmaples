FROM node:14
WORKDIR /websocker
COPY package*.json ./
RUN npm install
COPY . .
EXPOSE 9899/tcp
EXPOSE 9899/udp
CMD [ "node", "index.js" ]