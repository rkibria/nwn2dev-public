﻿/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

    INWNMasterServerAPI.cs

Abstract:

    This module houses the interface definition for the INWNMasterServerAPI web
    service.

--*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

namespace NWN
{
    /// <summary>
    /// This interface represents the primary API surface for the NWN Master
    /// Server system.
    /// </summary>
    [ServiceContract(Namespace = "http://api.mst.valhallalegends.com/NWNMasterServerAPI")]
    public interface INWNMasterServerAPI
    {

        /// <summary>
        /// Search the server database for a server by name, and return any
        /// matching online servers (zero or more).
        /// </summary>
        /// <param name="Product">Supplies the product name, such as NWN2.</param>
        /// <param name="ServerName">Supplies the server name.</param>
        /// <returns>A list of matching online servers is returned.</returns>
        [OperationContract]
        IList<NWGameServer> LookupServerByName(string Product, string ServerName);

        /// <summary>
        /// Search the server database for a server by address, and return stored
        /// information about it (the server may be offline).
        /// </summary>
        /// <param name="Product">Supplies the product name, such as NWN2.</param>
        /// <param name="ServerAddress">Supplies the server address.  This value
        /// takes the form of "ip:port".</param>
        /// <returns>The matching server.</returns>
        [OperationContract]
        NWGameServer LookupServerByAddress(string Product, string ServerAddress);

        /// <summary>
        /// Search the server database for all servers that are online for the
        /// given game product.
        /// </summary>
        /// <param name="Product">Supplies the product name, such as NWN2.</param>
        /// <returns>A list of matching online servers is returned.</returns>
        [OperationContract]
        IList<NWGameServer> GetOnlineServerList(string Product);
    }

    /// <summary>
    /// This class represents data information associated with a NWN game server.
    /// </summary>
    [DataContract]
    public class NWGameServer
    {
        /// <summary>
        /// The product name ("NWN1", "NWN2").
        /// </summary>
        [DataMember]
        public string Product { get; set; }

        /// <summary>
        /// Bitmask of expansions required.
        /// 0x00 - No expansions.
        /// 0x01 - XP1.
        /// 0x02 - XP2.
        /// </summary>
        [DataMember]
        public uint ExpansionsMask { get; set; }

        /// <summary>
        /// The game server software build number.
        /// </summary>
        [DataMember]
        public uint BuildNumber { get; set; }

        /// <summary>
        /// The name of the loaded module (if any).
        /// </summary>
        [DataMember]
        public string ModuleName { get; set; }

        /// <summary>
        /// The name of the server (if any).
        /// </summary>
        [DataMember]
        public string ServerName { get; set; }

        /// <summary>
        /// The current count of players logged on to the server.
        /// </summary>
        [DataMember]
        public uint ActivePlayerCount { get; set; }

        /// <summary>
        /// The maximum configured limit on concurrent players for the server.
        /// </summary>
        [DataMember]
        public uint MaximumPlayerCount { get; set; }

        /// <summary>
        /// True if the server is a local vault server, else false for a server
        /// vault server.
        /// </summary>
        [DataMember]
        public bool LocalVault { get; set; }

        /// <summary>
        /// The last UTC time that the server responded to a heartbeat message.
        /// </summary>
        [DataMember]
        public DateTime LastHeartbeat { get; set; }

        /// <summary>
        /// The hostname and port information used by player clients to connect to
        /// the server directly.  The information is returned in the format of
        /// "ip:port".
        /// </summary>
        [DataMember]
        public string ServerAddress { get; set; }

        /// <summary>
        /// True if the server is currently online.
        /// </summary>
        [DataMember]
        public bool Online { get; set; }

        /// <summary>
        /// True if the server is private and requires a player password in order
        /// to connect.
        /// </summary>
        [DataMember]
        public bool PrivateServer { get; set; }
    }
}