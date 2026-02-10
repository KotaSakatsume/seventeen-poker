NAME        = 17poker
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++17 -Iinclude

SRCS_DIR    = src
OBJS_DIR    = obj

# srcディレクトリ内の全ての.cppファイルを対象にする
SRCS        = $(wildcard $(SRCS_DIR)/*.cpp)
OBJS        = $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

# ソースコードからオブジェクトファイルを生成するルール
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re