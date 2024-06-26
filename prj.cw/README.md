# Курсовая работа "Распознавание дорожных знаков на изображениях"

### Инструкция по сборке

1. Склонировать репозиторий
```bash
git clone https://github.com/sssemion/misis2024s-21-02-sirotkin-s-u
cd misis2024s-21-02-sirotkin-s-u
mkdir build && cd build
```

2. Собрать проект
```bash
cmake .. && cmake --build .
```

3. Запустить бинарник
```bash
./bin/main --src <Путь к файлу с изображением> [--dst <Путь к файлу с размеченным изображением>] 
```

Чтобы запустить подсчет метрик выполнить:
```bash
./bin/metrics --data-dir ../prj.cw/src/main/metrics/data
```
