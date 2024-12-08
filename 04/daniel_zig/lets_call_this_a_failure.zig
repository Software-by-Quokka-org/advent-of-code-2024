const std = @import("std");

const Direction = enum {
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW,

    fn get(self: Direction) Point {
        switch (self) {
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
};
const State = enum(u8) {
    done,
    failed,
    M = 'M',
    A = 'A',
    S = 'S',
};

const Point = struct {
    x: isize,
    y: isize,

    pub fn add(a: Point, b: Point) Point {
        return .{
            .x = a.x + b.x,
            .y = a.y + b.y,
        };
    }
};

const Walker = struct {
    point: Point,
    direction: Direction,
    state: State,
    parent: *const Board,

    pub fn init(point: Point, parent: *Board) [8]?Walker {
        var walkers = [_]?Walker{null} ** 8;
        for (get_surround(point, parent.board), &walkers) |maybe_adj, *walker| {
            walker.* = if (maybe_adj) |adj| .{
                .point = adj.point,
                .direction = adj.direction,
                .state = .M,
                .parent = parent,
            } else null;
        }
        return walkers;
    }

    pub fn step(self: *Walker) ?bool {
        if (self.check()) {
            if (validate_point(
                self.point.add(self.direction.get()),
                self.parent.board,
            )) |next| {
                self.point = next;
            } else self.state = .failed;
        } else self.state = .failed;

        switch (self.state) {
            .M => self.state = .A,
            .A => self.state = .S,
            .S => {
                self.state = .done;
                return null;
            },
            .done => return null,
            .failed => return false,
        }

        return true;
    }

    fn check(self: Walker) bool {
        const current_char = self.parent.get_char(self.point) orelse return false;
        if (current_char == @intFromEnum(self.state)) {
            return true;
        } else {
            return false;
        }
    }

    const PointDirPair = struct { point: Point, direction: Direction };
    fn get_surround(point: Point, board: [][]const u8) [8]?PointDirPair {
        var points = [_]?PointDirPair{null} ** 8;
        const tags = std.meta.fields(Direction);
        inline for (tags, 0..) |tag, i| {
            const direction: Direction = @enumFromInt(tag.value);
            const adj_point = validate_point(point.add(direction.get()), board);
            if (adj_point != null) points[i] = .{ .point = adj_point.?, .direction = direction };
        }

        return points;
    }

    fn validate_point(point: Point, board: [][]const u8) ?Point {
        const y_len = board.len;
        const x_len = board[0].len;
        if (0 <= point.x and point.x < x_len and
            0 <= point.y and point.y < y_len)
        {
            return point;
        } else return null;
    }
};

const Board = struct {
    board: [][]const u8 = undefined,
    walkers: []Walker = undefined,

    pub fn init(string: []const u8, allocator: std.mem.Allocator) !Board {
        var this = Board{};
        var board = std.ArrayList([]const u8).init(allocator);
        var walkers = std.ArrayList(Walker).init(allocator);

        var row_iter = std.mem.tokenizeScalar(u8, string, '\n');
        while (row_iter.next()) |row| {
            try board.append(row);
        }
        this.board = try board.toOwnedSlice();

        for (0..this.board.len) |y| {
            for (0..this.board[0].len) |x| {
                const char = this.board[y][x];
                if (char == 'X') {
                    for (Walker.init(.{ .x = @intCast(x), .y = @intCast(y) }, &this)) |maybe_walker| {
                        if (maybe_walker) |walker| try walkers.append(walker);
                    }
                }
            }
        }
        this.walkers = try walkers.toOwnedSlice();
        return this;
    }

    pub fn get_char(self: Board, point: Point) ?u8 {
        if (point.y < 0 or self.board.len <= point.y) return null;
        if (point.x < 0 or self.board.len <= point.x) return null;

        const x: usize = @intCast(point.x);
        const y: usize = @intCast(point.y);

        return self.board[y][x];
    }
};

pub fn main() void {
    const input = @embedFile("input");
    std.debug.print("Part1: {}\n", .{part1(input)});
}

fn part1(input: []const u8) u32 {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    var arena = std.heap.ArenaAllocator.init(gpa.allocator());
    defer _ = arena.deinit();

    var count: u32 = 0;
    const board = Board.init(input, arena.allocator()) catch unreachable;
    for (board.walkers) |*walker| {
        validation: while (walker.step()) |validate| {
            if (validate == false) break :validation;
        } else count += 1;
    }
    return count;
}

test part1 {
    const only_mas =
        \\........
        \\........
        \\..MAS...
        \\........
        \\........
        \\........
        \\........
        \\........
    ;
    try std.testing.expectEqual(0, part1(only_mas));

    const partial_xmas =
        \\........
        \\........
        \\..XM....
        \\........
        \\........
        \\........
        \\........
        \\........
    ;
    try std.testing.expectEqual(0, part1(partial_xmas));

    const single_xmas =
        \\........
        \\........
        \\..XMAS..
        \\........
        \\........
        \\........
        \\........
        \\........
    ;
    try std.testing.expectEqual(1, part1(single_xmas));

    const single_with_overlap =
        \\........
        \\...X....
        \\..XMAS..
        \\........
        \\........
        \\........
        \\........
        \\........
    ;
    try std.testing.expectEqual(1, part1(single_with_overlap));

    const double_with_overlap =
        \\........
        \\...X....
        \\..XMAS..
        \\...A....
        \\...S....
        \\........
        \\........
        \\........
    ;
    try std.testing.expectEqual(2, part1(double_with_overlap));

    const multiple_with_overlap =
        \\....XXMAS.
        \\.SAMXMS...
        \\...S..A...
        \\..A.A.MS.X
        \\XMASAMX.MM
        \\X.....XA.A
        \\S.S.S.S.SS
        \\.A.A.A.A.A
        \\..M.M.M.MM
        \\.X.X.XMASX
    ;
    try std.testing.expectEqual(18, part1(multiple_with_overlap));

    const multiple_with_obfuscation =
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
    try std.testing.expectEqual(18, part1(multiple_with_obfuscation));
}
