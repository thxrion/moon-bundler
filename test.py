from bundle import create_bundle, read_file, get_bundle_code, get_bundle_path

TEST_EXAMPLES = [
  ".\\test\\project1\\project1.lua",
  ".\\test\\project2\\project2.lua",
  ".\\test\\project3\\project3.lua",
]

def test_bundler(file_path):
  real_output = get_bundle_code(file_path)
  expected_output = read_file(get_bundle_path(file_path))
  return real_output == expected_output

fails = []
for test_example in TEST_EXAMPLES:
  if not test_bundler(test_example):
    fails.append("TEST FAILED: {0}".format(test_example))

if len(fails) > 0:
  for fail in fails:
    print(fail)
  print("\nFAILED TESTS: {0}".format(len(fails)))
else:
  print("ALL TESTS WERE SUCCESSFULLY PASSED")
