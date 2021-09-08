﻿using JetBrains.Annotations;
using JetBrains.Collections.Viewable;
using JetBrains.Rd.Base;
using JetBrains.Rd.Impl;

namespace JetBrains.Rd
{
  public interface IProtocol : IRdDynamic
  {
    [NotNull] ViewableSet<RdExtBase> OutOfSyncModels { get; }
    [NotNull] string Name { get; }
    [NotNull] ISerializers Serializers { get; }
    [NotNull] IIdentities Identities { get; }
    [NotNull] IScheduler Scheduler { get; }
    [NotNull] IWire Wire { get; }    
    [NotNull] ProtocolContexts Contexts { get; }
    [NotNull] RdSignal<CreatedExtInfo> ExtCreatedNetworked { get; }
    [NotNull] Signal<CreatedExtInfo> ExtCreatedLocal { get; }
  }
}
