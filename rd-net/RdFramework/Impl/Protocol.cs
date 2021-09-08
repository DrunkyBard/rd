using System;
using System.Collections.Generic;
using JetBrains.Annotations;
using JetBrains.Collections.Viewable;
using JetBrains.Diagnostics;
using JetBrains.Lifetimes;
using JetBrains.Rd.Base;

namespace JetBrains.Rd.Impl
{
  public class Protocol : IProtocol
  {
    public static readonly ILog Logger = Log.GetLog("protocol");
    public static readonly ILog InitLogger = Logger.GetSublogger("INIT");
    public static LogWithLevel? InitTrace = InitLogger.Trace();

    /// <summary>
    /// Should match textual RdId of protocol intern root in Kotlin/js/cpp counterpart
    /// </summary>
    const string ProtocolInternRootRdId = "ProtocolInternRoot";
    const string ContextHandlerRdId = "ProtocolContextHandler";
    internal const string ProtocolExtCreatedRdId = "ProtocolExtCreated";
    
    /// <summary>
    /// Should match whatever is in rd-gen for ProtocolInternScope
    /// </summary>
    const string ProtocolInternScopeStringId = "Protocol";

    public Protocol([NotNull] string name, [NotNull] ISerializers serializers, [NotNull] IIdentities identities, [NotNull] IScheduler scheduler, 
      [NotNull] IWire wire, Lifetime lifetime, params RdContextBase[] initialContexts) 
      : this(name, serializers, identities, scheduler, wire, lifetime, null, null, null, null, initialContexts)
    { }

    internal Protocol([NotNull] string name, [NotNull] ISerializers serializers, [NotNull] IIdentities identities, [NotNull] IScheduler scheduler,
      [NotNull] IWire wire, Lifetime lifetime, SerializationCtx? serializationCtx = null, [CanBeNull] ProtocolContexts parentContexts = null, 
      [CanBeNull] Signal<CreatedExtInfo> parentExtCreatedLocal = null, [CanBeNull] RdSignal<CreatedExtInfo> parentExtCreatedNetworked = null, params RdContextBase[] initialContexts)
    {
      
      Name = name ?? throw new ArgumentNullException(nameof(name));
      Location = new RName(name);

      Serializers = serializers ?? throw new ArgumentNullException(nameof(serializers));
      Identities = identities ?? throw new ArgumentNullException(nameof(identities));
      Scheduler = scheduler ?? throw new ArgumentNullException(nameof(scheduler));
      Wire = wire ?? throw new ArgumentNullException(nameof(wire));
      SerializationContext = serializationCtx ?? new SerializationCtx(this, new Dictionary<string, IInternRoot<object>>() {{ProtocolInternScopeStringId, CreateProtocolInternRoot(lifetime)}});
      Contexts = parentContexts ?? new ProtocolContexts(SerializationContext);
      wire.Contexts = Contexts;
      if (serializationCtx == null)
        SerializationContext.InternRoots[ProtocolInternScopeStringId].Bind(lifetime, this, ProtocolInternRootRdId);
      foreach (var rdContextBase in initialContexts) rdContextBase.RegisterOn(Contexts);
      if (parentContexts == null)
        BindContexts(lifetime);
      OutOfSyncModels = new ViewableSet<RdExtBase>();
      ExtCreatedLocal = parentExtCreatedLocal ?? new Signal<CreatedExtInfo>();
      ExtCreatedNetworked = parentExtCreatedNetworked ?? this.CreateExtSignal();
      scheduler.InvokeOrQueue(() =>
      {
        ExtCreatedNetworked.Bind(lifetime, this, ProtocolExtCreatedRdId);
        ExtCreatedNetworked.Advise(lifetime, message =>
        {
          if (ExtCreatedNetworked.IsLocalChange) return;
          ExtCreatedLocal.Fire(message);
        });
      });
      
      if (wire is IWireWithDelayedDelivery wireWithMessageBroker)
        wireWithMessageBroker.StartDeliveringMessages();
    }

    private InternRoot<object> CreateProtocolInternRoot(Lifetime lifetime)
    {
      var root = new InternRoot<object>();
      root.RdId = RdId.Nil.Mix(ProtocolInternRootRdId);
      
      return root;
    }

    private void BindContexts(Lifetime lifetime)
    {
      Contexts.RdId = RdId.Nil.Mix(ContextHandlerRdId);
      Scheduler.InvokeOrQueue(() =>
      {
        if (!lifetime.IsAlive) return;
        Contexts.Bind(lifetime, this, ContextHandlerRdId);
      });
    }
      
    public string Name { get; }
    
    public IWire Wire { get; }
    public ISerializers Serializers { get; }
    public IIdentities Identities { get; }
    public IScheduler Scheduler { get; }
    public SerializationCtx SerializationContext { get; }
    public ViewableSet<RdExtBase> OutOfSyncModels { get; }

    public ProtocolContexts Contexts { get; }
    
    public RdSignal<CreatedExtInfo> ExtCreatedNetworked { get; }
    public Signal<CreatedExtInfo> ExtCreatedLocal { get; }

    [PublicAPI] public bool ThrowErrorOnOutOfSyncModels = true;
    
    
    public RName Location { get; }
    IProtocol IRdDynamic.Proto => this;

  }
}