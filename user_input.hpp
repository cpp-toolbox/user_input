#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <functional>
#include <string>
#include <vector>

#include "sbpt_generated_includes.hpp"

std::string get_validated_input(std::function<std::string()> input_func,
                                std::function<bool(const std::string &)> is_valid, const std::string &invalid_message);

std::string get_input_with_default(const std::string &prompt, const std::string &default_value);

bool get_yes_no(const std::string &prompt = "Please enter yes or no");

std::vector<std::string> select_options_from_dict(const std::unordered_map<std::string, std::string> &options_dict,
                                                  bool single_select = false, bool allow_all = true);

std::vector<std::string> select_options(const std::vector<std::string> &options, bool single_select);

#endif // USER_INPUT_HPP
