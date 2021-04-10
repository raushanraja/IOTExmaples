FROM node:14
WORKDIR /frontend
COPY package*.json ./
RUN npm install
COPY . .
EXPOSE 9052/tcp
EXPOSE 9052/udp
CMD [ "npm", "start" ]