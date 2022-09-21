@RuleNode(
        type = ComponentType.FILTER,
        name = "check key",
        relationTypes = {"True", "False"},
        configClazz = TbKeyFilterNodeConfiguration.class,
        nodeDescription = "Checks the existence of the selected key in the message payload.",
        nodeDetails = "If the selected key exists - send Message via <b>True</b> chain, otherwise <b>False</b> chain is used.",
        uiResources = {"static/rulenode/custom-nodes-config.js"},
        configDirective = "tbFilterNodeCheckKeyConfig")
public class TbKeyFilterNode implements TbNode {

    private static final ObjectMapper mapper = new ObjectMapper();

    private TbKeyFilterNodeConfiguration config;
    private String key;


    @Override
    public void init(TbContext tbContext, TbNodeConfiguration configuration) throws TbNodeException {
        this.config = TbNodeUtils.convert(configuration, TbKeyFilterNodeConfiguration.class);
        key = config.getKey();
    }

    @Override
    public void onMsg(TbContext ctx, TbMsg msg) {
        try {
            ctx.tellNext(msg, mapper.readTree(msg.getData()).has(key) ? "True" : "False");
        } catch (IOException e) {
            ctx.tellFailure(msg, e);
        }
    }

    @Override
    public void destroy() {
    }
}