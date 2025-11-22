# Используем легкий образ nginx
FROM nginx:alpine

# Копируем все файлы проекта в контейнер
COPY . /usr/share/nginx/html/

# Копируем конфигурацию nginx
COPY nginx.conf /etc/nginx/conf.d/default.conf

# Открываем порт 80
EXPOSE 80

# Команда запуска nginx (уже встроена в базовый образ)
CMD ["nginx", "-g", "daemon off;"]