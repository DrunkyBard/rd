using System;
using System.Collections.Generic;
using System.Threading;
using JetBrains.Diagnostics;

namespace JetBrains.Util;

public class SimpleObjectPool<T>
{
  private readonly int myMaxCapacity;
  private readonly Func<T> myFactory;
  private readonly Action<T>? myClear;
  private readonly Stack<T> myPool;
  private readonly Thread myThread;

  public SimpleObjectPool(Func<T> factory, Action<T>? clear) : this(int.MaxValue, factory, clear)
  {
  }

  public SimpleObjectPool(int maxCapacity,  Func<T> factory, Action<T>? clear)
  {
    myMaxCapacity = maxCapacity;
    myFactory = factory;
    myClear = clear;
    myPool = new Stack<T>();
    myThread = Thread.CurrentThread;
  }

  public T Rent()
  {
    Assertion.AssertCurrentThread(myThread);
    
    var pool = myPool;
    return pool.Count > 0 ? pool.Pop() : myFactory();
  }

  public RentedValueCookie<T> RentCookie() => new(this, Rent());

  public void Return(T value)
  {
    Assertion.AssertCurrentThread(myThread);
    
    var pool = myPool;
    if (pool.Count < myMaxCapacity)
    {
      myClear?.Invoke(value);
      pool.Push(value);
    }
  }
} 

public sealed class SimpleListPool<T> : SimpleObjectPool<List<T>>
{
  public SimpleListPool(int maxCapacity) : base(maxCapacity, () => new List<T>(), list => list.Clear())
  {
  }
}

public readonly ref struct RentedValueCookie<T>
{
  private readonly SimpleObjectPool<T> myPool;
  public T Value { get; }

  public RentedValueCookie(SimpleObjectPool<T> pool, T value)
  {
    myPool = pool;
    Value = value;
  }

  public void Dispose() => myPool.Return(Value);
} 
