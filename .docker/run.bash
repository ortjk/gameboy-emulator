docker run -it --rm \
--mount src=/home/thomas/Projects/gameboy-emulator,target=/workspace,type=bind \
-u cmake-container:docker cmake:latest