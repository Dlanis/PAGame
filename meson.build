project('PAGame', 'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3',
                     'cpp_std=c++20',
                     'unity_size=1000',
                     'b_pie=true',
                     'b_ndebug=if-release',
                     'buildtype=debugoptimized'])

headers = run_command(['python3', 'glob.py', 'hpp', './src'], check: true).stdout().substring(0, -1).split('\0')
sources = run_command(['python3', 'glob.py', 'cpp', './src'], check: true).stdout().substring(0, -1).split('\0')

global_args = [
  '-gsplit-dwarf',
]

add_global_arguments(global_args, language: ['cpp'])

deps = []

fmt_proj = subproject('fmt')
fmt_dep  = fmt_proj.get_variable('fmt_dep')
deps += fmt_dep

entt_proj = subproject('entt')
entt_dep  = entt_proj.get_variable('entt_dep')
deps += entt_dep

glm_proj = subproject('glm')
glm_dep  = glm_proj.get_variable('glm_dep')
deps += glm_dep

komihash_proj = subproject('komihash')
komihash_dep  = komihash_proj.get_variable('komihash_dep')
deps += komihash_dep

glaze_proj = subproject('glaze')
glaze_dep  = glaze_proj.get_variable('glaze_dep')
deps += glaze_dep

unordered_dense_proj = subproject('unordered_dense')
unordered_dense_dep  = unordered_dense_proj.get_variable('unordered_dense_dep')
deps += unordered_dense_dep

# raylib_proj = subproject('raylib')
# raylib_dep  = raylib_proj.get_variable('raylib_dep')
# glfw_dep  = raylib_proj.get_variable('glfw_dep')
# deps += raylib_dep

raylib_dep = dependency('raylib')
deps += raylib_dep

inc = include_directories('src')

exe = executable('pagame', [headers, sources],
  cpp_pch: 'pch/pagame.hpp',
  install: true,
  dependencies: deps,
  include_directories: inc,
)
