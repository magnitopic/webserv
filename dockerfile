# Use the official Nginx base image
FROM nginx:latest


# Copy custom Nginx configuration file from the host to the container
# COPY webserv.conf /etc/nginx/conf.d/webserv.conf

# Expose port 80 for Nginx
EXPOSE 80

# Start Nginx server in the foreground
CMD ["nginx", "-g", "daemon off;"]
