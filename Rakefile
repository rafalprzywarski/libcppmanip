require 'rake/clean'
require './buildpath'

CLOBBER.include(BUILD_DIRECTORY)


task :default => :test 
task :test => [:unit_tests, :acceptance_tests]

task :acceptance_tests => :cppmanip do
  sh 'cucumber -p acceptance_tests'
end

task :wip => :cppmanip do
  sh 'cucumber -p wip'
end

task :ci_acceptance_tests => :cppmanip do
  sh 'cucumber -p ci_acceptance_tests'
end

task :cppmanip do
  sh "cmake -E make_directory #{BUILD_DIRECTORY}"
  sh "cmake -E chdir #{BUILD_DIRECTORY} cmake .."
  sh "cmake --build #{BUILD_DIRECTORY} --target cppmaniprunner_extract_method"
end

task :cppmaniptest do
  sh "cmake -E make_directory #{BUILD_DIRECTORY}"
  sh "cmake -E chdir #{BUILD_DIRECTORY} cmake .."
  sh "cmake --build #{BUILD_DIRECTORY} --target cppmaniptest"
end

task :cppmaniprunnertest do
  sh "cmake -E make_directory #{BUILD_DIRECTORY}"
  sh "cmake -E chdir #{BUILD_DIRECTORY} cmake .."
  sh "cmake --build #{BUILD_DIRECTORY} --target cppmaniprunnertest"
end

task :unit_tests => [ :cppmaniptest, :cppmaniprunnertest ] do
  sh "#{BUILD_DIRECTORY}/library/cppmaniptest"
  sh "#{BUILD_DIRECTORY}/runner/cppmaniprunnertest"
end
 
task :ci_unit_tests => :cppmaniptest do
  sh "#{BUILD_DIRECTORY}/cppmaniptest --gtest_output=xml:ci_test_results/"
end