#include "HetrickCV.hpp"
#include "dsp/digital.hpp"

struct TwoToFour : Module 
{
	enum ParamIds 
	{
		NUM_PARAMS
	};
	enum InputIds 
	{
        INA_INPUT,
        INB_INPUT,
		NUM_INPUTS
	};
	enum OutputIds 
	{
        OUT1_OUTPUT,
        OUT2_OUTPUT,
        OUT3_OUTPUT,
        OUT4_OUTPUT,
		NUM_OUTPUTS
	};

    float outs[4] = {};

	TwoToFour() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) 
	{
		
	}

	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - reset, randomize: implements special behavior when user clicks these from the context menu
};


void TwoToFour::step() 
{
    const float inA = inputs[INA_INPUT].value;
    const float inB = inputs[INB_INPUT].value;

    outs[0] = inA + inB;
    outs[1] = outs[0] * -1.0f;
    outs[3] = inA - inB;
    outs[2] = outs[3] * -1.0f;

    outputs[OUT1_OUTPUT].value = outs[0];
    outputs[OUT2_OUTPUT].value = outs[1];
    outputs[OUT3_OUTPUT].value = outs[2];
    outputs[OUT4_OUTPUT].value = outs[3];
}


TwoToFourWidget::TwoToFourWidget() 
{
	auto *module = new TwoToFour();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		auto *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/2To4.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 365)));

    //////PARAMS//////

    //////INPUTS//////
    addInput(createInput<PJ301MPort>(Vec(10, 100), module, TwoToFour::INA_INPUT));
    addInput(createInput<PJ301MPort>(Vec(55, 100), module, TwoToFour::INB_INPUT));

    //////OUTPUTS//////
    addOutput(createOutput<PJ301MPort>(Vec(33, 150), module, TwoToFour::OUT1_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(33, 195), module, TwoToFour::OUT2_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(33, 240), module, TwoToFour::OUT3_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(33, 285), module, TwoToFour::OUT4_OUTPUT));

    //////BLINKENLIGHTS//////
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(70, 158), &module->outs[0]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(70, 203), &module->outs[1]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(70, 248), &module->outs[2]));
    addChild(createValueLight<SmallLight<GreenRedPolarityLight>>(Vec(70, 293), &module->outs[3]));
}