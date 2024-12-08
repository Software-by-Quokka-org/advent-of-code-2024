const std = @import("std");

pub fn main() void {
    const input = @embedFile("input");
    std.debug.print("Part1: {}\n", .{part1(input)});
    std.debug.print("Part2: {}\n", .{part2(input)});
}

fn mul(operands: []const u8) ?u32 {
    const end = std.mem.indexOfScalar(u8, operands, ')') orelse return null;
    const separator = std.mem.indexOfScalar(u8, operands, ',') orelse return null;
    if (separator > end) return null;

    const first = operands[0..separator];
    const second = operands[separator + 1 .. end];
    if (!valid(first) or !valid(second)) return null;

    const a = std.fmt.parseInt(u32, first, 10) catch unreachable;
    const b = std.fmt.parseInt(u32, second, 10) catch unreachable;
    return a * b;
}

fn valid(str: []const u8) bool {
    for (str) |c| {
        if (c < '0' or '9' < c) return false;
    }
    return true;
}

fn part1(input: []const u8) u32 {
    var mul_iter = std.mem.tokenizeSequence(u8, input, "mul(");
    var sum: u32 = 0;
    while (mul_iter.next()) |operands| {
        sum += mul(operands) orelse continue;
    }
    return sum;
}

test part1 {
    const test_input = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
    try std.testing.expectEqual(161, part1(test_input));
}

fn part2(input: []const u8) u32 {
    var sum: u32 = 0;
    var buffer: []const u8 = input;
    while (true) {
        var next_do = std.mem.indexOf(u8, buffer, "do()");
        next_do = if (next_do) |i| i + 4 else null;

        var next_dont = std.mem.indexOf(u8, buffer, "don't()");
        next_dont = if (next_dont) |i| i + 7 else null;

        var next_mul = std.mem.indexOf(u8, buffer, "mul(") orelse break;
        next_mul += 4;

        if (next_do != null and next_do.? < next_mul) {
            buffer = buffer[next_do.?..];
            continue;
        }

        if (next_dont != null and next_dont.? < next_mul) {
            buffer = if (next_do) |next| buffer[next..] else break;
            continue;
        }

        buffer = buffer[next_mul..];
        sum += mul(buffer) orelse continue;
    }
    return sum;
}

test part2 {
    const test_input = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
    try std.testing.expectEqual(48, part2(test_input));
}
