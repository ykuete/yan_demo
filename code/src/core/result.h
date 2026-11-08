#pragma once

#include <QString>

#include <optional>
#include <utility>

namespace quadm {

// Why an operation failed. The GUI uses the code to decide what to do (e.g. disable editing for
// Unsupported); the message is always safe to show to the user.
enum class ErrorCode {
    NotFound,           // the file or folder does not exist
    Unsupported,        // no backend can handle this file type / operation
    ReadOnly,           // the field or the file may not be changed
    BackendUnavailable, // a helper program (ExifTool) could not be started
    Timeout,            // a helper program did not finish in time
    ParseError,         // a helper program printed something we do not understand
    WriteFailed,        // the backend ran but reported a problem
    InvalidArgument,
};

struct Error {
    ErrorCode code = ErrorCode::InvalidArgument;
    QString message;
};

// Outcome of an operation that returns nothing. Replaces "bool + QString &error" out-parameters:
// the failure cannot be forgotten about and the message travels with it.
class Status
{
public:
    Status() = default; // success
    Status(Error error) : m_error(std::move(error)), m_ok(false) {}

    static Status success() { return Status(); }
    static Status failure(ErrorCode code, QString message)
    {
        return Status(Error{code, std::move(message)});
    }

    bool isOk() const { return m_ok; }
    explicit operator bool() const { return m_ok; }
    const Error &error() const { return m_error; }
    QString message() const { return m_error.message; }

private:
    Error m_error;
    bool m_ok = true;
};

// Outcome of an operation that returns a T. Holds either a value or an Error.
template <typename T> class Result
{
public:
    Result(T value) : m_value(std::move(value)) {}
    Result(Error error) : m_error(std::move(error)) {}

    static Result failure(ErrorCode code, QString message)
    {
        return Result(Error{code, std::move(message)});
    }

    bool isOk() const { return m_value.has_value(); }
    explicit operator bool() const { return isOk(); }

    // Only valid when isOk().
    const T &value() const { return *m_value; }
    T &value() { return *m_value; }
    T takeValue() { return std::move(*m_value); }
    T valueOr(T fallback) const { return m_value ? *m_value : std::move(fallback); }

    const Error &error() const { return m_error; }
    QString message() const { return m_error.message; }

    // Drops the value; handy to propagate a failure of a different type.
    Status status() const { return isOk() ? Status::success() : Status(m_error); }

private:
    std::optional<T> m_value;
    Error m_error;
};

} // namespace quadm
