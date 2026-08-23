#pragma once

template <typename T>
class TCollection {
public:
  virtual ~TCollection() = default;

  // Shared concepts for ALL collections
  virtual int GetSize() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual void Clear() = 0;
};