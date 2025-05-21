CPP = g++
STANDART = -std=c++17
CFLAGS_CPP = -Wall -Wextra -Werror $(STANDART)
GTEST = -lgtest
GCOV = -fprofile-arcs -ftest-coverage
SF = -fsanitize=leak,address
DIST_DIR := archive
BUILD_DIR_TEST = build_test

BACKEND_SRC =  ./3DViewer_back/back/affine.cpp ./3DViewer_back/back/parser.cpp ./3DViewer_back/mc/model.cpp ./3DViewer_back/mc/controller.cpp
MATRIX_SRC = ./matrix/s21_matrix_oop.cpp
TEST_SRC = ./tests/*.cpp

BACKEND_OBJECT = $(BACKEND_SRC:.cpp=.o)
MATRIX_OBJECT = $(MATRIX_SRC:.cpp=.o)

all: install

install: desktop

desktop:
	mkdir desk
	cd desk && qmake ../View
	cd desk && make -j$(nproc)
	mkdir -p build
	mv desk/3DViewer build
	rm -rf desk

desktop_clean:
	rm -rf desk
	make desktop

lib:
	sudo apt install qt5-dev

libmatrix.a: $(MATRIX_OBJECT)
	ar rc libmatrix.a $(MATRIX_OBJECT)
	ranlib libmatrix.a
	rm -rf $(MATRIX_OBJECT)

libbackend.a: $(BACKEND_OBJECT)
	ar rc libbackend.a $(BACKEND_OBJECT)
	ranlib libbackend.a
	rm -rf $(BACKEND_OBJECT)

run:
	chmod 777 ./build/3DViewer
	./build/3DViewer

dvi:
	@xdg-open dvi/Report.md
.PHONY: dvi

dist: clean_dist
	@cd ../ && mkdir -p ${DIST_DIR}
	@cd ../ && cp -rf src/3DViewer_back ${DIST_DIR}/3DViewer_back
	@cd ../ && cp -rf src/View ${DIST_DIR}/View
	@cd ../ && cp -rf src/Makefile ${DIST_DIR}/
	@cd ../ && tar -czvf archive.tar.gz ${DIST_DIR}
	@cd ../ && rm -rf ${DIST_DIR}

dist_unpack:
	@cd ../ && tar -xzvf archive.tar

ifeq ($(shell uname), Linux)
    test: clean libmatrix.a libbackend.a
		$(CPP) $(CFLAGS_CPP) $(TEST_SRC) -L. -lmatrix -lbackend $(GTEST) -o tests/tests
		./tests/tests

else ifeq ($(shell uname), Darwin)
    test:
		cd tests && mkdir -p $(BUILD_DIR_TEST)
		cd tests && cd $(BUILD_DIR_TEST) && cmake ..
		cd tests && cd $(BUILD_DIR_TEST) && make tests && ./tests

endif

gcov : clean libmatrix.a
	$(CPP) -g $(CFLAGS_CPP) $(TEST_SRC) $(BACKEND_SRC) -L. -lmatrix $(GTEST) $(GCOV) -o tests/tests
	./tests/tests
	mkdir ./tests/gcov_report
	gcovr -r . --html --html-details -o ./tests/gcov_report/index.html
	gcovr -r . --txt -o ./tests/coverage_report.txt
	rm tests/*.gcda tests/*.gcno

	@xdg-open ./tests/gcov_report/index.html

gcov_clean:
	rm -rf tests/gcov_report/ tests/tests tests/coverage_report.txt

clean:
	rm -rf *.o *.a build 3DViewer_back/*.o
	rm -rf $(MATRIX_OBJ) $(BACKEND_OBJECT)
	rm -rf desk
	# rm -rf tests/gcov_report/ tests/tests tests/coverage_report.txt

uninstall:
	rm -rf build

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz