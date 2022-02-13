all:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lSOIL -lobj -lopengl32 -lglu32 -lglut32 -lm -o cube.exe -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lobj -lSDL2 -lSDL2_image -lGL -lm -o cube -Wall -Wextra -Wpedantic
