const std = @import("std");

pub fn main() !void {
    const input = @embedFile("input");
    std.debug.print("Part1: {}\n", .{try part1(input)});
    std.debug.print("Part2: {}\n", .{try part2(input)});
}

fn parse_list(input: []const u8, allocator: anytype) !struct { []u32, []u32 } {
    var left = std.ArrayList(u32).init(allocator);
    var right = std.ArrayList(u32).init(allocator);

    var row_iter = std.mem.tokenizeScalar(u8, input, '\n');
    while (row_iter.next()) |row| {
        var elements = std.mem.tokenizeScalar(u8, row, ' ');
        try left.append(try std.fmt.parseInt(u32, elements.next().?, 10));
        try right.append(try std.fmt.parseInt(u32, elements.next().?, 10));
    }

    return .{ try left.toOwnedSlice(), try right.toOwnedSlice() };
}

fn part1(input: []const u8) !u32 {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    const left, const right = try parse_list(input, allocator);
    std.mem.sort(u32, left, {}, std.sort.asc(u32));
    std.mem.sort(u32, right, {}, std.sort.asc(u32));

    var sum: u32 = 0;
    for (left, right) |l, r| {
        sum += @max(l, r) - @min(l, r);
    }

    return sum;
}

fn part2(input: []const u8) !u32 {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    const left, const right = try parse_list(input, allocator);
    var lookup = std.AutoHashMap(u32, u32).init(allocator);
    for (right) |r| {
        try lookup.put(r, if (lookup.contains(r)) lookup.get(r).? + 1 else 1);
    }

    var sum: u32 = 0;
    for (left) |l| {
        if (lookup.get(l)) |r_count| {
            sum += l * r_count;
        }
    }

    return sum;
}

test part1 {
    const test_input =
        \\3   4
        \\4   3
        \\2   5
        \\1   3
        \\3   9
        \\3   3
    ;

    try std.testing.expectEqual(11, part1(test_input));
}

test part2 {
    const test_input =
        \\3   4
        \\4   3
        \\2   5
        \\1   3
        \\3   9
        \\3   3
    ;

    try std.testing.expectEqual(31, part2(test_input));
}
