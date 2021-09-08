﻿using System;
using JetBrains.Annotations;
using JetBrains.Diagnostics;
using JetBrains.Rd.Base;
using JetBrains.Serialization;

namespace JetBrains.Rd.Impl
{
  public static class ExtCreatedUtils
  {
    [NotNull]
    public static RdSignal<CreatedExtInfo> CreateExtSignal(this IRdDynamic @this)
    {
      var signal = new RdSignal<CreatedExtInfo>(
        (ctx, reader) =>
        {
          var rName = ReadRName(reader);
          var rdId = reader.ReadNullableStruct((_, r) => r.ReadRdId(), ctx);
          var hash = reader.ReadLong();
          return new CreatedExtInfo(rName, rdId, hash);
        },
        (ctx, writer, value) =>
        {
          WriteRName(writer, value.Name);
          writer.WriteNullableStruct((_, w, v) => w.Write(v), ctx, value.Id);
          writer.Write(value.Hash);
        }
      );
      var baseId = @this is IRdWireable wireable ? wireable.RdId : RdId.Nil;
      signal.RdId = baseId.Mix(Protocol.ProtocolExtCreatedRdId);
      return signal;
    }
    
    [NotNull]
    private static RName ReadRName([NotNull] UnsafeReader reader)
    {
      var rootName = reader.ReadString() ?? throw new InvalidOperationException();
      var last = reader.ReadBoolean();
      var rName = new RName(rootName);
      while (!last)
      {
        var separator = reader.ReadString() ?? throw new InvalidOperationException();
        var localName = reader.ReadString() ?? throw new InvalidOperationException();
        last = reader.ReadBoolean();
        rName = rName.Sub(localName, separator);
      }
      return rName;
    }

    private static void WriteRName([NotNull] UnsafeWriter writer, [NotNull] RName value)
    {
      TraverseRName(value, true, (rName, last) =>
      {
        if (rName == RName.Empty) return;
        if (rName.Parent != RName.Empty)
        {
          writer.Write(rName.Separator);
        }
        writer.Write(rName.LocalName.ToString());
        writer.Write(last);
      });
    }

    private static void TraverseRName([NotNull] RName rName, bool last, [NotNull] Action<RName, bool> handler)
    {
      if (rName.Parent is RName rParent)
      {
        TraverseRName(rParent, false, handler);
      }
      handler(rName, last);
    }
  }
}