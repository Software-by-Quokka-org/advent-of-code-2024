const std = @import("std");
const assert = std.debug.assert;

const xmas_seq = "XMAS";

const Direction = enum { N, NE, E, SE, S, SW, W, NW };
fn rel_point(dir: Direction) Point {
    switch (dir) {
        .N => return .{ .x = 0, .y = -1 },
        .NE => return .{ .x = 1, .y = -1 },
        .E => return .{ .x = 1, .y = 0 },
        .SE => return .{ .x = 1, .y = 1 },
        .S => return .{ .x = 0, .y = 1 },
        .SW => return .{ .x = -1, .y = 1 },
        .W => return .{ .x = -1, .y = 0 },
        .NW => return .{ .x = -1, .y = -1 },
    }
}

const Point = struct {
    x: isize,
    y: isize,

    pub fn to_index(self: Point, string: []const u8, row_len: usize) ?usize {
        const index_calc = (self.y * @as(isize, @intCast(row_len))) + self.x + self.y;

        if (index_calc < 0) return null;
        const index: usize = @intCast(index_calc);

        if (index >= string.len) return null;
        return index;
    }

    pub fn from_index(index: usize, row_len: usize) Point {
        const y: isize = @intCast(index / row_len);
        const x = @as(isize, @intCast(index % row_len)) - y;
        return Point{ .x = x, .y = y };
    }

    pub fn add(a: Point, b: Point) Point {
        return Point{
            .x = a.x + b.x,
            .y = a.y + b.y,
        };
    }

    pub fn mul(a: Point, b: isize) Point {
        return Point{
            .x = a.x * b,
            .y = a.y * b,
        };
    }
};

test "Point.to_index" {
    const string =
        \\abc
        \\def
    ;
    const row_len = 3;
    const point = Point{ .x = 1, .y = 1 };
    const point_index = point.to_index(string, row_len);
    try std.testing.expect(point_index != null);
    try std.testing.expectEqual('e', string[point_index.?]);
}

test "Point.to_index out of bounds" {
    const string =
        \\abc
        \\def
    ;
    const row_len = 3;
    const point = Point{ .x = 5, .y = 1 };
    const point_index = point.to_index(string, row_len);
    try std.testing.expectEqual(null, point_index);
}

test "Point.from_index" {
    const index = 5;
    const row_len = 3;
    const point = Point.from_index(index, row_len);
    try std.testing.expectEqual(1, point.x);
    try std.testing.expectEqual(1, point.y);
}

test "Point.add" {
    const a = Point{ .x = 1, .y = 1 };
    const b = Point{ .x = 1, .y = 1 };
    const res = a.add(b);
    try std.testing.expectEqual(2, res.x);
    try std.testing.expectEqual(2, res.y);
}

test "Point.mul" {
    const a = Point{ .x = -1, .y = 1 };
    const b = 2;
    const res = a.mul(b);
    try std.testing.expectEqual(-2, res.x);
    try std.testing.expectEqual(2, res.y);
}

fn count_xmas(string: []const u8, index: usize) u32 {
    const row_len = std.mem.indexOf(u8, string, "\n") orelse string.len;
    var found: u32 = 0;
    _ = &found;

    inline for (std.meta.fields(Direction)) |tag| {
        const direction: Direction = @enumFromInt(tag.value);
        const start_point = Point.from_index(index, row_len);

        for (xmas_seq, 0..) |char, i| {
            const next_point_rel = rel_point(direction).mul(@intCast(i));
            const check_point = start_point.add(next_point_rel);
            const check_index = check_point.to_index(string, row_len) orelse break;

            if (string[check_index] != char) break;
        } else found += 1;
    }

    return found;
}

pub fn main() void {
    const input = @embedFile("input");
    std.debug.print("Part1: {}\n", .{part1(input)});
    std.debug.print("Part2: {}\n", .{part2(input)});
}

fn part1(input: []const u8) u32 {
    var sum: u32 = 0;
    for (input, 0..) |char, i| {
        if (char != 'X') continue;
        sum += count_xmas(input, i);
    }
    return sum;
}

test "part1 partial test input" {
    const input =
        \\........
        \\..XM....
        \\........
    ;
    try std.testing.expectEqual(0, part1(input));
}

test "part1 small test input" {
    const input =
        \\........
        \\..XMAS..
        \\........
    ;
    try std.testing.expectEqual(1, part1(input));
}

test "part1 full test input" {
    const input =
        \\MMMSXXMASM
        \\MSAMXMSMSA
        \\AMXSXMAAMM
        \\MSAMASMSMX
        \\XMASAMXAMM
        \\XXAMMXXAMA
        \\SMSMSASXSS
        \\SAXAMASAAA
        \\MAMMMXMMMM
        \\MXMXAXMASX
    ;
    try std.testing.expectEqual(18, part1(input));
}

fn valid_mas(string: []const u8, index: usize) bool {
    const row_len = std.mem.indexOf(u8, string, "\n") orelse string.len;
    const directions = .{ .{ Direction.NW, Direction.SE }, .{ Direction.SW, Direction.NE } };
    const start_point = Point.from_index(index, row_len);

    inline for (directions) |dir_pair| {
        const point_one = rel_point(dir_pair[0]).add(start_point);
        const point_two = rel_point(dir_pair[1]).add(start_point);

        const index_one = point_one.to_index(string, row_len) orelse return false;
        const index_two = point_two.to_index(string, row_len) orelse return false;

        const char_one = string[index_one];
        const char_two = string[index_two];

        if (char_one == char_two) return false;
        if (char_one != 'M' and char_one != 'S') return false;
        if (char_two != 'M' and char_two != 'S') return false;
    }

    return true;
}

test "valid_mas" {
    const string =
        \\M.S
        \\.A.
        \\M.S
    ;
    const index = std.mem.indexOf(u8, string, "A").?;
    try std.testing.expect(valid_mas(string, index));
}

fn part2(input: []const u8) u32 {
    var sum: u32 = 0;
    for (input, 0..) |char, i| {
        if (char != 'A') continue;
        if (valid_mas(input, i)) sum += 1;
    }

    return sum;
}

test "part2 full test input" {
    const input =
        \\MMMSXXMASM
        \\MSAMXMSMSA
        \\AMXSXMAAMM
        \\MSAMASMSMX
        \\XMASAMXAMM
        \\XXAMMXXAMA
        \\SMSMSASXSS
        \\SAXAMASAAA
        \\MAMMMXMMMM
        \\MXMXAXMASX
    ;
    try std.testing.expectEqual(9, part2(input));
}
