TEST_DIRS = sensor_test main_test
TARGET_DIRS = $(addprefix tests/, $(TEST_DIRS))
TARGET_FILES = $(addsuffix /test, $(TARGET_DIRS))
MAIN_TEST = main_test
MAIN_TARGET_TEST = $(addsuffix /test, $(addprefix tests/, $(MAIN_TEST)))


$(MAIN_TEST): $(TARGET_FILES)
	sudo ./$(MAIN_TARGET_TEST)

tests/%:
	make -C $(dir $@)

run: $(TARGET_DIRS)
	sudo ./$(MAIN_TARGET_TEST)

clean :
	@rm -rf $(TARGET_FILES)

