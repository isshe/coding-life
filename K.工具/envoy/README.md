
```
pc bazel build --sandbox_writable_path=/root/.ccache --compilation_mode=dbg -c opt envoy --verbose_failures --sandbox_writable_path="/run/user/$UID/ccache-tmp/" --sandbox_debug --copt=-fno-limit-debug-info --copt="-Wno-error"

bazel clean --expunge

yum install libstdc++-static

wget https://dl.rockylinux.org/pub/rocky/9/CRB/x86_64/os/Packages/l/libstdc++-static-11.3.1-4.3.el9.x86_64.rpm
yum install libstdc++-static-11.3.1-4.3.el9.x86_64.rpm
```

```
compile pkg: missing strict dependencies:
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/entity.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/enum.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/enum_value.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/extension.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/field.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/file.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/init_option.go: import of "github.com/spf13/afero"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/message.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/method.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/oneof.go: import of "google.golang.org/protobuf/runtime/protoimpl"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/persister.go: import of "github.com/spf13/afero"
	/root/.cache/bazel/_bazel_root/4643dbd8b634ef9663ca6c879c08d349/sandbox/linux-sandbox/574/execroot/envoy/external/com_github_lyft_protoc_gen_star/service.go: import of "google.golang.org/protobuf/runtime/protoimpl"
No dependencies were provided.
```

```
bazel fetch //source/exe:envoy
bazel query --output label_kind '//external:*' 2>&1 | grep com_github_lyft_protoc_gen_star
```
