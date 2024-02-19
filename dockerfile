FROM nginx

RUN rm /etc/nginx/conf.d/default.conf

COPY webserv.conf /etc/nginx/conf.d/

COPY pages/ /etc/nginx/pages/

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
