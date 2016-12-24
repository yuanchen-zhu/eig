This is stripped from production code, so it might not compile by itself yet.
The original production code compiles under Visual C++ 2015, and uses C++11 (and
maybe some C++14).

The code implements the following enhancement to the API of Eigen.

* Alias types, functions, and enum flags to follow a more Standard C++
  Library/Boost naming convention. These aliases include member functions,
  member types, and member constants as well. Exmpales: `eig::mat` instead of
  `Eigen::Matrix`, `mat<...>::static_rows` instead of
  `Matrix<...>::RowsAtCompileTime`, and so on.
  
* Allow construct from list literals. Examples:

```cpp
TEST(mat, construct_from_2d_list_literal)
{
    auto ok = MatEq({{1, 0}, {0, 1}});

    auto dynamic_sized = mat<int>{{1, 0}, {0, 1}};
    EXPECT_THAT(dynamic_sized, ok);

    mat<int, 2, 2> fix_sized({{1, 0}, {0, 1}});
    EXPECT_THAT(fix_sized, ok);

    auto fix_cols = mat<int, eig::dynamic, 2>{{1, 0}, {0, 1}};
    auto fix_rows = mat<int, 2, eig::dynamic>{{1, 0}, {0, 1}};
    EXPECT_THAT(fix_cols, ok);
    EXPECT_THAT(fix_rows, ok);
}

TEST(mat, construct_from_list_literal_of_rows)
{
    auto ok = MatEq({{1, 2}, {3, 4}});
    vec<int> row1{1, 2};
    vec<int> row2{3, 4};

    mat<int, 2, 2> fix_sized{row1, row2};
    EXPECT_THAT(fix_sized, ok);

    auto dynamic_sized = mat<int>{row1, row2};
    EXPECT_THAT(dynamic_sized, ok);

    auto mixed_rows_fixed = mat2i{row1, vec2i{3, 4}};
    EXPECT_THAT(mixed_rows_fixed, ok);

    auto mixed_rows_dynamic = mat<int>{{1, 2}, row2};
    EXPECT_THAT(mixed_rows_dynamic, ok);
}

TEST(mat, vec_construct_from_1d_list_literal)
{
    auto dynamic_sized = vec<int>{1, 2};
    EXPECT_THAT(dynamic_sized, VecEq({1, 2}));

    auto fix_sized = vec<int, 2>{1, 2};
    EXPECT_THAT(fix_sized, VecEq({1, 2}));

    auto long_than_4_fix_sized = vec<int, 6>{1, 2, 3, 4, 5, 6};
    EXPECT_THAT(long_than_4_fix_sized, VecEq({1, 2, 3, 4, 5, 6}));
}
```

* When construct a dynamically size matrix from sizes, use the `from_size` tag
  to avoid confusion with value initialization. Example:

```cpp
// using from_size constructor tag
TEST(mat, construct_from_size)
{
    /// [construct_from_size]
    auto m = mat<int>{from_size, 1, 2};
    EXPECT_EQ(m.rows(), 1);
    EXPECT_EQ(m.cols(), 2);

    auto v = vec<int>{from_size, 2};
    EXPECT_EQ(v.rows(), 2);
    EXPECT_EQ(v.cols(), 1);

    auto w = row_vec<int>{from_size, 2};
    EXPECT_EQ(w.rows(), 1);
    EXPECT_EQ(w.cols(), 2);
    /// [construct_from_size]
}
```
  
  
* Expose free member functions for quite a few of Eigen member functions.
  Examples:

```cpp
TEST(mat, free_standing_fun)
{
    vec3f u{1.f, 2.f, 3.f};
    vec3f v{-1.f, 3.f, 6.f};

    EXPECT_FLOAT_EQ(u.norm(), norm(u));
    EXPECT_FLOAT_EQ(u.dot(v), dot(u, v));
    EXPECT_TRUE(is_approx(u.cross(v), cross(u, v)));
}
```

* Allow simpler subblock access syntax inspired by matlab that replaces the ugly
  army of block(), XXXCorner(), XXXRows(), XXXCols(), head(), tail(),
  and segment() functions. Examples:
  
```cpp
TEST(mat, block_acesss_example)
{
    /// [block_access_example]
    mat<int> m{{1, 2, 3}, // simple 2x3 matrix
               {4, 5, 6}};

    // Single entry lookup is not changed. Notice zero indexing.
    EXPECT_EQ(m(1, 1), 5);

    // Entire matrix
    EXPECT_THAT(m(all, all), MatEq({{1, 2, 3}, {4, 5, 6}}));

    // Second column
    EXPECT_THAT(m(all, 1), MatEq({{2}, {5}}));

    // First 1 row, first 2 columns. The returned block has static row count and
    // dynamic column count.
    EXPECT_THAT(m(first<1>, first(2)), MatEq({{1, 2}}));

    // First row, last two columns
    EXPECT_THAT(m(0, last(2)), MatEq({{2, 3}}));

    // Row 1..2 inclusive, second column.
    EXPECT_THAT(m(range(0, 1), 1), MatEq({{2}, {5}}));

    // Row 1..2 inclusive, column 2..3, second column. The returned block
    // has dynamic row count and static column count
    EXPECT_THAT(m(range(0, 1), range<1, 2>), MatEq({{2, 3}, {5, 6}}));

    // Row 1..2 inclusive, second column. The row count of the returned block is
    // constant at compile time.
    EXPECT_THAT(m(static_sized_range<2>(0), 1), MatEq({{2}, {5}}));

    /// [block_access_example]
}

```
