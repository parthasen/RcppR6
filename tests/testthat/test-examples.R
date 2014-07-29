source("helper-rcppr6.R")

context("examples")

test_that("examples", {
  ## Because of the devtools issue (hadley/devtools#531) we need to use
  ## a non-standard temporary file location for the tests.
  path <- "~/tmp"

  file.copy(rcppr6_file("examples"), path, recursive=TRUE)
  pkg <- file.path(path, "examples")

  rcppr6::install(pkg)
  devtools::document(pkg)
  devtools::load_all(pkg)
  ## fresh=TRUE here would be nice, but can't happen.
  devtools::test(pkg)
  ## Should always clean up here, really.  Will go away when the
  ## tempdir issue does.
  unlink(pkg, recursive=TRUE)
})