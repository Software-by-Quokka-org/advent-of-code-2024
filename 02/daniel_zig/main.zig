const std = @import("std");

const Allocator = std.mem.Allocator;

const Direction = enum {
    up,
    down,
};

const Report = struct {
    items: []i32,
    allocator: Allocator = undefined,
    current: usize = 0,
    skip_index: ?usize = null,
    direction: Direction = undefined,

    pub fn init(row: []const u8, allocator: Allocator) Report {
        var row_iter = std.mem.tokenizeScalar(u8, row, ' ');
        var len: usize = 0;
        while (row_iter.next()) |_| {
            len += 1;
        }

        row_iter.reset();
        var items = allocator.alloc(i32, len) catch @panic("Out of memory!");
        var normalized_change: i32 = 0;
        for (0..len) |i| {
            const num = row_iter.next().?;
            items[i] = std.fmt.parseInt(i32, num, 10) catch unreachable;
            if (i == 0) continue;

            if (items[i - 1] < items[i]) {
                normalized_change += 1;
            } else if (items[i - 1] > items[i]) {
                normalized_change -= 1;
            }
        }

        return .{ .items = items, .allocator = allocator, .direction = if (normalized_change > 0) .up else .down };
    }

    pub fn deinit(self: Report) void {
        self.allocator.free(self.items);
    }

    pub fn next(self: *Report) ?i32 {
        const res = self.get(self.current);
        self.current = self.l_index(self.current + 1);
        return res;
    }

    pub fn peek(self: Report) ?i32 {
        return self.get(self.l_index(self.current) + 1);
    }

    pub fn reset(self: *Report) void {
        self.current = 0;
    }

    fn get(self: Report, index: usize) ?i32 {
        const i = self.l_index(index);
        if (i >= self.items.len) return null;
        return self.items[i];
    }

    fn l_index(self: Report, index: usize) usize {
        return if (self.skip_index == null or index != self.skip_index.?) index else index + 1;
    }
};

pub fn main() !void {
    const input = @embedFile("input");
    std.debug.print("Part1: {}\n", .{try part1(input)});
    std.debug.print("Part2: {}\n", .{try part2(input)});
}

fn check_rules(current: i32, next: i32, dir: Direction) bool {
    const diff: i32 = next - current;
    var success = true;

    success = if (@abs(diff) > 3) false else success;
    switch (dir) {
        .up => success = if (diff > 0) success else false,
        .down => success = if (diff < 0) success else false,
    }

    return success;
}

fn check_report(report_iter: *Report) bool {
    var next = report_iter.peek().?;
    while (report_iter.next()) |num| {
        const success = check_rules(num, next, report_iter.direction);

        if (!success) {
            return false;
        }

        next = report_iter.peek() orelse break;
    }
    return true;
}

fn part1(input: []const u8) !u32 {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    var safe_count: u32 = 0;
    var row_iter = std.mem.tokenizeScalar(u8, input, '\n');

    while (row_iter.next()) |row| {
        var report_iter = Report.init(row, gpa.allocator());
        defer report_iter.deinit();

        if (check_report(&report_iter)) safe_count += 1;
    }

    return safe_count;
}

fn part2(input: []const u8) !u32 {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    var safe_count: u32 = 0;
    var row_iter = std.mem.tokenizeScalar(u8, input, '\n');

    while (row_iter.next()) |row| {
        var report_iter = Report.init(row, gpa.allocator());
        defer report_iter.deinit();

        if (check_report(&report_iter)) {
            safe_count += 1;
            continue;
        }

        for (0..report_iter.items.len) |i| {
            report_iter.reset();
            report_iter.skip_index = i;

            if (check_report(&report_iter)) {
                safe_count += 1;
                break;
            }
        }
    }

    return safe_count;
}

test part1 {
    const test_input =
        \\7 6 4 2 1
        \\1 2 7 8 9
        \\9 7 6 2 1
        \\1 3 2 4 5
        \\8 6 4 4 1
        \\1 3 6 7 9
    ;

    try std.testing.expectEqual(2, part1(test_input));
}

test part2 {
    const test_input =
        \\3 6 4 2 1
        \\1 2 7 8 9
        \\9 7 6 2 1
        \\1 3 2 4 5
        \\8 6 4 4 1
        \\1 3 6 7 9
    ;

    try std.testing.expectEqual(4, part2(test_input));
}
