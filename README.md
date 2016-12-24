This is stripped from production code, so might not compile yet. The original
production code compiles under Visual C++ 2015. It uses C++11 (and maybe some
C++14).

The codes wraps implements the following features to enhance the API interface
of Eigen.

* Alias types, functions, and enum flags to follow a more Standard C++
  Library/Boost naming convention. These aliases include member functions,
  member types, and member constants, e.g., `eig::mat` instead of
  `Eigen::Matrix`, `mat::static_rows` instead of
  `Matrix<...>::RowsAtCompileTime`, and so on.
  
* Expose free member functions for quite a bunch of Eigen member functions.
  Examples:

```
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
  
```
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
