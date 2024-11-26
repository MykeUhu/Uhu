// Copyright by MykeUhu

using UnrealBuildTool;
using System.Collections.Generic;

public class UhuEditorTarget : TargetRules
{
	public UhuEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Uhu" } );
	}
}
