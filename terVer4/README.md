# лабка по терверу

Запускаем баш

    bash ./run.bash

Если интерпратор выдаст ошибку,вроде

    ./run.bash: line 3: pyuic5: command not found

То установим все, пип можно не обновлять, если не хочется

    python -m pip install --upgrade pip
    pip install PyQt5
    pip install PyHamcrest
