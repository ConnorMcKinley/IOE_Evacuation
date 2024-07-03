// Fill out your copyright notice in the Description page of Project Settings.


#include "UGraphFunctions.h"

void UUGraphFunctions::InitializeNodeConnections(const TArray<FNodeConnection>& Connections, TMap<int32, FNodeConnections>& OutNodeConnections)
{
	OutNodeConnections.Empty();

	for (const FNodeConnection& Connection : Connections) {
		FNodeConnections& Node1Connections = OutNodeConnections.FindOrAdd(Connection.NodeIndex1);
		Node1Connections.ConnectedNodes.AddUnique(Connection.NodeIndex2);
		FNodeConnections& Node2Connections = OutNodeConnections.FindOrAdd(Connection.NodeIndex2);
		Node2Connections.ConnectedNodes.AddUnique(Connection.NodeIndex1);
	}
	ump_NodeToRoutes = OutNodeConnections;

}

void UUGraphFunctions::AddNodeConnection(const FNodeConnection& Connection, TMap<int32, FNodeConnections>& OutNodeConnections)
{
	FNodeConnections& Node1Connections = OutNodeConnections.FindOrAdd(Connection.NodeIndex1);
	Node1Connections.ConnectedNodes.AddUnique(Connection.NodeIndex2);
	FNodeConnections& Node2Connections = OutNodeConnections.FindOrAdd(Connection.NodeIndex2);
	Node2Connections.ConnectedNodes.AddUnique(Connection.NodeIndex1);

	ump_NodeToRoutes = OutNodeConnections;
}

TMap<int32, FNodeConnections>& UUGraphFunctions::GetNodeToRoutes()
{
	return ump_NodeToRoutes;
}
