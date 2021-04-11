FROM node:14
WORKDIR /frontend
COPY package*.json ./
RUN npm install
COPY . .
ENV TZ=Asia/Kolkata
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
EXPOSE 9052/tcp
EXPOSE 9052/udp
CMD [ "npm", "start" ]