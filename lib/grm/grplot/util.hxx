#ifndef UTIL_HXX_INCLUDED
#define UTIL_HXX_INCLUDED

#include <optional>
#include <string>

#if !(defined(__EXCEPTIONS) || defined(__cpp_exceptions) || defined(_CPPUNWIND))
#define NO_EXCEPTIONS
#endif

namespace util
{
template <class... T> void unused(T &&...) {}

class GetExecutablePathError : public virtual std::exception
{
public:
  [[nodiscard]] const char *what() const noexcept override = 0;
};

class SetGrDirError : public virtual std::exception
{
public:
  [[nodiscard]] const char *what() const noexcept override = 0;
};

#if defined __unix__ || defined __APPLE__
class ErrnoError : public virtual std::exception
{
public:
  ErrnoError();
  [[nodiscard]] const char *what() const noexcept override;

private:
  std::string whatStr_;
};
#elif defined _WIN32
class FormatMessageError : public std::exception
{
public:
  [[nodiscard]] const char *what() const noexcept override;
};

class GetLastErrorError : public virtual std::exception
{
public:
  GetLastErrorError();
  [[nodiscard]] const char *what() const noexcept override;

private:
  std::string whatStr_;
};
#endif

#if defined __APPLE__
class PathTooLongError : public GetExecutablePathError
{
public:
  PathTooLongError(size_t neededSize);
  [[nodiscard]] const char *what() const noexcept override;

private:
  std::string whatStr_;
};
#elif defined __unix__
class PathTooLongError : public GetExecutablePathError
{
public:
  PathTooLongError();
  [[nodiscard]] const char *what() const noexcept override;

private:
  std::string whatStr_;
};

class ProcessFileLinkNotReadableError : public ErrnoError, public GetExecutablePathError
{
public:
  [[nodiscard]] const char *what() const noexcept override;
};
#endif

#if defined __unix__ || defined __APPLE__
class AbsolutePathError : public ErrnoError, public SetGrDirError
{
public:
  [[nodiscard]] const char *what() const noexcept override;
};

class SetEnvError : public ErrnoError, public SetGrDirError
{
public:
  [[nodiscard]] const char *what() const noexcept override;
};
#elif defined _WIN32
class WideCharToMultiByteError : public GetLastErrorError, public SetGrDirError
{
public:
  [[nodiscard]] const char *what() const noexcept override;
};

class ModulePathError : public GetLastErrorError, public SetGrDirError
{
public:
  [[nodiscard]] const char *what() const noexcept override;
};

class DirnameError : public SetGrDirError
{
public:
  DirnameError(const std::wstring &filepath);
  [[nodiscard]] const char *what() const noexcept override;

private:
  std::string whatStr_;
};

class AbsolutePathError : public SetGrDirError
{
public:
  AbsolutePathError(const std::wstring &path);
  [[nodiscard]] const char *what() const noexcept override;

private:
  std::string whatStr_;
};

class SetEnvError : public GetLastErrorError, public SetGrDirError
{
public:
  [[nodiscard]] const char *what() const noexcept override;
};
#endif

bool endsWith(const std::string &str, const std::string &suffix);
bool startsWith(const std::string &str, const std::string &prefix);

#ifdef NO_EXCEPTIONS
#ifdef _WIN32
std::optional<std::wstring> get_executable_path();
#else
std::optional<std::string> get_executable_path();
#endif
#else
#ifdef _WIN32
std::wstring getExecutablePath();
#else
std::string getExecutablePath();
#endif
#endif
#ifdef NO_EXCEPTIONS
bool
#else
void
#endif
setGrdir(bool force = false);
} // namespace util

#endif /* ifndef UTIL_HXX_INCLUDED */
