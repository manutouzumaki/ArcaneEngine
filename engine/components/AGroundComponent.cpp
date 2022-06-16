#include "AGroundComponent.h"

void AGroundComponent::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("AGroundComponent");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("AGroundComponent");
    root->LinkEndChild(type);
}
