class_name TestFile
extends Node3D

func _ready() -> void:
    print("Hello)


# This is a test function, for testing
func _this_is_a_test() -> bool:
    pass


## Hello there, I am testing parameters
func _this_tests_parameters(param_with_type : bool, param_without_type):
    pass
    ## This tests that indented comments won't be added


## I am testing
## multiline comments
# with weird formatting too
func this_tests_multiline_comments() -> void:
    pass