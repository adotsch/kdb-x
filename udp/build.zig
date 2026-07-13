const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const entry = b.option([]const u8, "ENTRY", "main C source file") orelse @panic("ENTRY build option is required");

    const lib = b.addSharedLibrary(.{
        .name = "init",
        .target = target,
        .optimize = optimize,
    });

    lib.root_module.strip = true;
    lib.link_gc_sections = true;

    lib.addCSourceFile(.{
        .file = b.path(entry),
        .flags = &.{ "-std=c11", "-fvisibility=hidden", "-ffunction-sections", "-fdata-sections" },
    });

    lib.addIncludePath(b.path("."));
    lib.linkLibC();

    lib.linker_allow_shlib_undefined = true;

    if (target.result.os.tag == .windows) {
        lib.linkSystemLibrary("ws2_32");
        lib.linkSystemLibrary("iphlpapi");
        if (target.result.cpu.arch == .x86_64) {
            lib.addObjectFile(b.path("libq_x64.a"));
        } else if (target.result.cpu.arch == .x86) {
            lib.addObjectFile(b.path("libq_x86.a"));
        } else if (target.result.cpu.arch == .aarch64) {
            lib.addObjectFile(b.path("libq_arm64.a"));
        }
    }

    b.installArtifact(lib);
}
