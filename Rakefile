require 'rake/clean'
require './buildpath'

CLOBBER.include(GMOCK_BUILD_DIRECTORY)
CLOBBER.include(LIBRARY_BUILD_DIRECTORY)
CLOBBER.include(LIBRARY_INSTALLATION_DIRECTORY)
CLOBBER.include(RUNNER_BUILD_DIRECTORY)

task :default => :acceptance_tests

task :gmock do
  build_cmake_targets GMOCK_BUILD_DIRECTORY, [ :gtest, :gmock ]
end

task :libcppmanip => :gmock do
  build_cmake_targets LIBRARY_BUILD_DIRECTORY, [ :cppmaniptest ]
  sh "#{LIBRARY_BUILD_DIRECTORY}/cppmaniptest"
end

task :install_libcppmanip => :libcppmanip do
  sh "(cd #{LIBRARY_BUILD_DIRECTORY}; make install DESTDIR=#{absolute_path(LIBRARY_INSTALLATION_DIRECTORY)})"
end

task :libcppmanip_runner => :install_libcppmanip do
  env = {
    :LIBCPPMANIP_INSTALLATION_INCLUDE => absolute_path(LIBCPPMANIP_INSTALLATION_INCLUDE),
    :LIBCPPMANIP_INSTALLATION_LIB => absolute_path(LIBCPPMANIP_INSTALLATION_LIB)
  }
  build_cmake_targets RUNNER_BUILD_DIRECTORY, [ :cppmaniprunner_extract_function, :cppmaniprunnertest ], env
  sh "#{RUNNER_BUILD_DIRECTORY}/cppmaniprunnertest"
end


task :acceptance_tests => :libcppmanip_runner do
  sh 'cucumber -p acceptance_tests'
end

task :wip => :libcppmanip_runner do
  sh 'cucumber -p wip'
end


PROJECT_DIRECTORY=File.dirname(__FILE__)

def absolute_path dir
  PROJECT_DIRECTORY + "/" + dir
end

def build_cmake_target path, target, env = {}
  sh "cmake -E make_directory #{path}"
  sh "cmake -E chdir #{path} cmake .. #{env.map{ |n,v| " -D#{n.to_s}=#{v}"}.join}"
  sh "cmake --build #{path} --target #{target.to_s}"
end

def build_cmake_targets path, targets, env = {}
  targets.each { |target| build_cmake_target path, target, env }
end

